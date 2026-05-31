//source file
#include "../include/Mutator_Nest_Lambda_With_Fold_In_Template_489.h"

// ========================================================================================================
#define MUT489_OUTPUT 1

void MutatorFrontendAction_489::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      // Check if lambda contains a fold expression
      bool hasFoldExpr = false;
      if (MT->getBody()) {
        auto range = MT->getBody()->children();
        for (auto it = range.begin(); it != range.end(); ++it) {
          if (*it && (*it)->getStmtClass() == clang::Stmt::BinaryOperatorClass || 
                     (*it)->getStmtClass() == clang::Stmt::UnaryOperatorClass) {
            hasFoldExpr = true;
            break;
          }
        }
      }
      if (!hasFoldExpr) return;

      //Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      
      // Check if we need to create a new template function
      std::string templateFuncName = "mutate_func_489";
      bool needNewTemplate = true;
      for (const auto& func : createdTemplateFunctions) {
        if (func == templateFuncName) {
          needNewTemplate = false;
          break;
        }
      }
      
      // Perform mutation
      std::string mutatedCode;
      if (needNewTemplate) {
        mutatedCode = "template<int = 0>\nvoid " + templateFuncName + "() {\n";
        createdTemplateFunctions.push_back(templateFuncName);
      } else {
        mutatedCode = "";
      }
      
      mutatedCode += "    auto outer = []() {\n";
      mutatedCode += "        auto inner = " + lambdaText + ";\n";
      mutatedCode += "        inner.template operator()<1,2,3>();\n";
      mutatedCode += "    };\n";
      mutatedCode += "    outer();\n";
      
      if (needNewTemplate) {
        mutatedCode += "}\n\nvoid caller_489() {\n";
        mutatedCode += "    " + templateFuncName + "();\n";
        mutatedCode += "}\n";
      }
      
      mutatedCode = "/*mut489*/" + mutatedCode;
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(MT->getEndLoc(), "\n" + mutatedCode);
    }
}
  
void MutatorFrontendAction_489::MutatorASTConsumer_489::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}