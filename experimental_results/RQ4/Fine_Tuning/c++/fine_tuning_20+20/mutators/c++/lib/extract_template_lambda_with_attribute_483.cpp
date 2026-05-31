//source file
#include "../include/Extract_Template_Lambda_with_Attribute_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateLambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << content << '\n';
      if (content.find("[]") != string::npos &&
          content.find("[[") != string::npos) {
        auto lambda_name = "l";
        if (content.find("auto l") == string::npos)
          lambda_name = "mut_483";
        auto lambda_def = content.substr(content.find("[]"), content.rfind('}') + 1 - content.find("[]"));
        llvm::outs() << lambda_def << '\n';
        lambda_def = string("template <typename T> auto ") + lambda_name + "=" +
                     lambda_def + ";\n";
        content.replace(content.find("[]"), content.rfind('}') + 1 - content.find("[]"), lambda_name + string("<T>()"));
        content = "/*mut483*/" + lambda_def + content;
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}