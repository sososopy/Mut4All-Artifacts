//source file
#include "../include/Introduce_Dependent_Template_In_Function_Body_233.h"

// ========================================================================================================
#define MUT233_OUTPUT 1

void MutatorFrontendAction_233::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunctions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      if (!MT->isTemplateInstantiation())
        return;
      if (!MT->hasBody())
        return;
      if (MT->isFunctionTemplateSpecialization())
        return;
      if (MT->getDescribedFunctionTemplate() == nullptr)
        return;
      auto params = MT->getTemplateSpecializationArgs();
      if (params == nullptr)
        return;
      auto args = params->asArray();
      if (args.size() == 0)
        return;
      auto arg0 = args[0].getAsType().getAsString();
      llvm::outs() << arg0 << '\n';
      auto body = MT->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      llvm::outs() << body_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      string ins = "/*mut233*/&" + arg0 + "<int>";
      if (body_str.rfind('}') != string::npos)
        body_str.insert(body_str.rfind('}'), ins);
      llvm::outs() << body_str << '\n';
      declaration = declaration.replace(
          declaration.find('{'), declaration.rfind('}') - 1, body_str);
      llvm::outs() << declaration << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_233::MutatorASTConsumer_233::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("TemplateFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}