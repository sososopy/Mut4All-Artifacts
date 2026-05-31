//source file
#include "../include/Add_Concept_Parameter_Require_Clause_68.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation())
        return;
      if (FD->isDependentContext())
        return;
      if (FD->isDefaulted())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isOverloadedOperator())
        return;
      if (FD->getDescribedFunctionTemplate() != nullptr)
        return;
      if (FD->getNumParams() == 0)
        return;
      //Get the source code text of target node
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getSourceRange());
      auto params = FD->parameters();
      bool has_concept = false;
      for (auto param : params) {
        if (param->getType()->isDependentType()) {
          has_concept = true;
          break;
        }
      }
      if (!has_concept)
        return;
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << definition << '\n';
      string new_definition = "";
      if (FD->doesThisDeclarationHaveABody()) {
        new_definition += definition.substr(0, definition.rfind(')') + 1);
        new_definition += '{';
        new_definition += definition.substr(definition.rfind('{') + 1);
      } else {
        new_definition += definition.substr(0, definition.rfind(')') + 1);
        new_definition += "{}";
      }
      llvm::outs() << new_definition << '\n';
      auto new_params = params;
      //Replace the original AST node with the mutated one
      for (auto param : new_params) {
        if (param->getType()->isDependentType()) {
          string param_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                     param->getSourceRange());
          string new_param_str = "require " + param_str;
          llvm::outs() << new_param_str << '\n';
          stringutils::strReplace(new_definition, param_str, new_param_str);
        }
      }
      llvm::outs() << new_definition << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(FD->getSourceRange()), new_definition);
    }
}
  
void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}