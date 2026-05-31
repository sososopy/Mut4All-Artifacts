//source file
#include "../include/Modify_Union_Member_Initialization_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
        //Filter nodes in header files
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;

        for (auto *field : UD->fields()) {
            if (field->hasInClassInitializer()) {
                //Get the source code text of target node
                auto initExpr = field->getInClassInitializer();
                if (initExpr) {
                    //Perform mutation on the source code text by applying string replacement
                    std::string mutatedInit = "a + \"string\"";
                    //Replace the original AST node with the mutated one
                    Rewrite.ReplaceText(initExpr->getSourceRange(), mutatedInit);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isUnion()).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}