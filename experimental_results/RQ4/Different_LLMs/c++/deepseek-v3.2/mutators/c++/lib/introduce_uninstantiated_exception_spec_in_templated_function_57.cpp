//source file
#include "../include/Mutator_Introduce_Uninstantiated_Exception_Spec_In_Templated_Function_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplatedFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isTemplated())
        return;
      auto *FT = FD->getType()->getAs<clang::FunctionProtoType>();
      if (!FT)
        return;
      if (FT->hasExceptionSpec())
        return;
      if (FD->getNumParams() == 0)
        return;
      //Get the source code text of target node
      auto funcText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = funcText.find('(');
      if (pos == std::string::npos)
        return;
      std::string paramName = FD->getParamDecl(0)->getNameAsString();
      if (paramName.empty())
        paramName = "param0";
      std::string noexceptSpec = " noexcept(noexcept(std::move(" + paramName + ")))";
      funcText.insert(pos, noexceptSpec);
      funcText = "/*mut57*/" + funcText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}
  
void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), unless(cxxConstructorDecl()), unless(cxxDestructorDecl())).bind("TemplatedFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}