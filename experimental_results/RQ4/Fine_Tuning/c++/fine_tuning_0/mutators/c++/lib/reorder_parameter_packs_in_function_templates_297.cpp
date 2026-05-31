//source file
#include "../include/Reorder_Parameter_Packs_In_Function_Templates_297.h"

// ========================================================================================================
#define MUT297_OUTPUT 1

void MutatorFrontendAction_297::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->getTemplatedDecl()->isFunctionOrFunctionTemplate())
        return;
      if (MT->getTemplateParameters()->size() <= 1)
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto params = MT->getTemplateParameters();
      std::vector<int> param_types;
      for (auto param : *params) {
        if (param->isTemplateParameterPack()) {
          param_types.push_back(1);
        } else {
          param_types.push_back(0);
        }
      }
      int n = param_types.size();
      int pack_index = -1;
      for (int i = 0; i < n; ++i) {
        if (param_types[i] == 1) {
          pack_index = i;
          break;
        }
      }
      if (pack_index == -1)
        return;
      if (pack_index == n - 1)
        return;
      std::string new_content;
      for (int i = 0; i < n; ++i) {
        if (i == pack_index)
          continue;
        if (i)
          new_content += ',';
        new_content += stringutils::rangetoStr(*(Result.SourceManager),
                                               params->getParam(i)->getSourceRange());
      }
      new_content += ',';
      new_content += stringutils::rangetoStr(*(Result.SourceManager),
                                             params->getParam(pack_index)->getSourceRange());
      llvm::outs() << new_content << '\n';
      content.replace(content.find('<'), content.rfind('>') - content.find('<') + 1,
                      '<' + new_content + '>');
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_297::MutatorASTConsumer_297::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}