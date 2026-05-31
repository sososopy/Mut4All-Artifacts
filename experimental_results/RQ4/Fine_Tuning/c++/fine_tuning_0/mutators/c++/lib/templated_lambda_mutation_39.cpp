//source file
#include "../include/Templated_Lambda_Mutation_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      if (LT->isGenericLambda() == false)
        return;
      //Get the source code text of target node
      auto lambda_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LT->getSourceRange());
      llvm::outs() << lambda_content << "\n";
      auto template_params = LT->getTemplateParameterList();
      if (template_params->size() == 0)
        return;
      auto lambda_params = LT->getLambdaClass()->getLambdaCallOperator();
      auto params = lambda_params->parameters();
      if (params.size() == 0)
        return;
      string param_name = params[0]->getNameAsString();
      string switch_stmt = "switch(" + param_name + "){\n";
      for (int i = 0; i < 2; ++i) {
        switch_stmt += "case " + std::to_string(i) + " : ";
        switch_stmt += lambda_params->getNameAsString() + ".operator()<";
        for (int j = 0; j < template_params->size(); ++j) {
          if (j > 0)
            switch_stmt += ",";
          switch_stmt += std::to_string(i);
        }
        switch_stmt += ">();\nbreak;\n";
      }
      switch_stmt += "}\n";
      llvm::outs() << switch_stmt << "\n";
      //Perform mutation on the source code text by applying string replacement
      lambda_content.insert(lambda_content.rfind('}'), switch_stmt);
      lambda_content = "/*mut39*/" + lambda_content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), lambda_content);
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}