//source file
#include "../include/Mutate_Reverse_Iterator_Parameters_In_Lambda_138.h"

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_138::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambda_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    MT->getSourceRange());
      llvm::outs() << lambda_content << '\n';
      //Record the node information to be used in the mutation process
      auto params = MT->getLambdaClass()->getLambdaCallOperator()->parameters();
      for (auto param : params) {
        auto type = param->getType();
        if (type->isRValueReferenceType() || type->isLValueReferenceType()) {
          auto pointee_type = type->getPointeeType();
          auto type_name = pointee_type.getAsString();
          if (type_name.find("reverse_iterator") != string::npos) {
            llvm::outs() << type_name << '\n';
            std::string new_type_name = type_name;
            if (getrandom::getRandomIndex(1)) {
              new_type_name.replace(type_name.find("reverse_iterator"),
                                    std::string("reverse_iterator").length(),
                                    "iterator");
            } else {
              new_type_name.replace(type_name.find("reverse_iterator"),
                                    std::string("reverse_iterator").length(),
                                    "const_iterator");
            }
            lambda_content.replace(lambda_content.find(type_name),
                                   type_name.length(), new_type_name);
          }
        }
      }
      //Perform mutation on the source code text by applying string replacement
      lambda_content = "/*mut138*/" + lambda_content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda_content);
    }
}
  
void MutatorFrontendAction_138::MutatorASTConsumer_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}