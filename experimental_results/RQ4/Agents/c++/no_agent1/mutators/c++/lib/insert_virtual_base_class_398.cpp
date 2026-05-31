//source file
#include "../include/insert_virtual_base_class_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        std::string className = CL->getNameAsString();
        if (visitedClasses.find(className) != visitedClasses.end())
            return;

        visitedClasses.insert(className);
        std::string virtualBaseClass = "class VirtualBase {};";

        // Perform mutation on the source code text by applying string replacement
        std::string insertion = "class Derived_" + className + " : virtual public " + className + ", virtual public VirtualBase {};\n";
        insertion = "/*mut398*/" + insertion;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfter(CL->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}