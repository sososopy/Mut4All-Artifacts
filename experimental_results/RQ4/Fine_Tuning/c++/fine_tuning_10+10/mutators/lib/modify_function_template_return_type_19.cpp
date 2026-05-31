//source file
#include "../include/Modify_Function_Template_Return_Type_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        //Filter nodes in header files
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                FT->getLocation()))
            return;

        for (auto *Spec : FT->specializations()) {
            if (Spec->getReturnType()->isUndeducedAutoType()) {
                //Get the source code text of target node
                auto funcDecl = Spec;
                std::string newReturnType = funcDecl->getReturnType().getAsString();
                newReturnType += "&"; // Change return type to reference

                //Perform mutation on the source code text by applying string replacement
                Rewrite.ReplaceText(funcDecl->getReturnTypeSourceRange(), newReturnType);

                // If necessary, implement a minimal body
                if (!funcDecl->hasBody()) {
                    std::string funcBody = "{ static " + newReturnType + " value{}; return value; }";
                    Rewrite.InsertTextAfterToken(funcDecl->getEndLoc(), funcBody);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(functionDecl(returns(autoType())))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}