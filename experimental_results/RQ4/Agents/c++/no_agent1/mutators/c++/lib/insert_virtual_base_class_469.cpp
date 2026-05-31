//source file
#include "../include/insert_virtual_base_class_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        //Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        // Record the class name
        auto className = CL->getNameAsString();
        if (visitedClasses.find(className) != visitedClasses.end())
            return;
        visitedClasses.insert(className);

        // Get the source code text of target node
        std::string insertion = "class VirtualBase469 {};\n";
        std::string baseClassInsertion = " : virtual public VirtualBase469";

        // Perform mutation on the source code text by applying string replacement
        if (CL->getDefinition() && CL->isThisDeclarationADefinition()) {
            SourceLocation startLoc = CL->getBeginLoc();
            if (CL->getNumBases() > 0) {
                auto baseSpecifier = CL->bases_begin();
                SourceLocation baseLoc = baseSpecifier->getBeginLoc();
                Rewrite.InsertText(baseLoc, baseClassInsertion + ", ", true, true);
            } else {
                Rewrite.InsertTextAfterToken(startLoc, baseClassInsertion);
            }
            SourceLocation endLoc = CL->getEndLoc();
            Rewrite.InsertTextAfterToken(endLoc, insertion);
        }
    }
}

void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}