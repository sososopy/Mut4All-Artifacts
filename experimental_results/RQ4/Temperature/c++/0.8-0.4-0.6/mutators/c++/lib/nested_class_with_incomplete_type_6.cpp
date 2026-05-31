//source file
#include "../include/nested_class_with_incomplete_type_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        //Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;

        if (!CL->isCompleteDefinition())
            return;

        //Get the source code text of target node
        auto className = CL->getNameAsString();
        if (className.empty())
            return;

        //Perform mutation on the source code text by applying string replacement
        std::string mutation = className + " self_member; /*mut6*/\n";
        auto insertLoc = CL->getBeginLoc().getLocWithOffset(1);
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextBefore(insertLoc, mutation);
    }
}

void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}