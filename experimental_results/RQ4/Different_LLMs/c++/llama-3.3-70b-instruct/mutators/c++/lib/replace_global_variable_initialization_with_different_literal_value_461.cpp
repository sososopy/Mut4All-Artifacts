//source file
#include "../include/Replace_Global_Variable_Initialization_With_Different_Literal_Value_461.h"

// ========================================================================================================
#define MUT461_OUTPUT 1

void MutatorFrontendAction_461::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VarDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VarDecl->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VarDecl->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (VarDecl->hasInit()) {
        auto init = VarDecl->getInit();
        if (auto *IntegerLiteral = dyn_cast<clang::IntegerLiteral>(init)) {
          int value = IntegerLiteral->getValue().getSExtValue();
          int newValue = value + 1; // replace with a different literal value
          declaration.replace(declaration.find(std::to_string(value)), std::to_string(value).length(), std::to_string(newValue));
        } else if (auto *CharacterLiteral = dyn_cast<clang::CharacterLiteral>(init)) {
          char value = CharacterLiteral->getValue();
          char newValue = value + 1; // replace with a different literal value
          declaration.replace(declaration.find(std::string(1, value)), std::string(1, value).length(), std::string(1, newValue));
        } else if (auto *FloatingLiteral = dyn_cast<clang::FloatingLiteral>(init)) {
          float value = FloatingLiteral->getValueAsApproximateDouble();
          float newValue = value + 1.0f; // replace with a different literal value
          declaration.replace(declaration.find(std::to_string(value)), std::to_string(value).length(), std::to_string(newValue));
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarDecl->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_461::MutatorASTConsumer_461::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasGlobalStorage(), hasInitializer(integerLiteral())).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}