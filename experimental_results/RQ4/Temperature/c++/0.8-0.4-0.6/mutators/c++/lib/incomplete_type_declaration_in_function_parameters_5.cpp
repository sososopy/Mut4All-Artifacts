//source file
#include "../include/incomplete_type_declaration_in_function_parameters_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->getNumParams() > 0) {
        auto *Param = FD->getParamDecl(0);
        auto ParamType = Param->getType().getAsString();
        std::string IncompleteTypeName = "IncompleteType";

        // Declare the incomplete struct type earlier in the code
        std::string IncompleteTypeDecl = "struct " + IncompleteTypeName + ";";
        Rewrite.InsertTextBefore(FD->getBeginLoc(), IncompleteTypeDecl + "\n");

        // Change the parameter type to an incomplete struct type
        std::string NewParamType = "struct " + IncompleteTypeName;
        Rewrite.ReplaceText(Param->getTypeSourceInfo()->getTypeLoc().getSourceRange(), NewParamType);
      }
    }
}
  
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isExpansionInMainFile()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}