//source file
#include "../include/Mutator_Introduce_Self_Referential_Trait_Constraint_120.h"

// ========================================================================================================
#define MUT120_OUTPUT 1

void MutatorFrontendAction_120::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CR = Result.Nodes.getNodeAs<clang::Expr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!CR || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CR->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto requiresText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CR->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      //Check if the requires clause contains a type trait like std::is_constructible_v
      if (requiresText.find("std::is_constructible_v") == std::string::npos)
        return;
      //Find the template parameter name F from the constructor
      auto *Parent = Result.Nodes.getNodeAs<clang::FunctionDecl>("Constructor");
      if (!Parent) return;
      auto *Templated = dyn_cast<clang::FunctionTemplateDecl>(Parent);
      if (!Templated) return;
      auto *TemplParams = Templated->getTemplateParameters();
      if (!TemplParams || TemplParams->size() == 0) return;
      //Assume the first template parameter is F
      std::string ParamName = TemplParams->getParam(0)->getNameAsString();
      if (ParamName.empty()) return;
      //Perform mutation on the source code text by applying string replacement
      //Replace the first argument of is_constructible2_v with std::decay_t<F>
      size_t pos = requiresText.find("std::is_constructible_v<");
      if (pos == std::string::npos) return;
      pos += strlen("std::is_constructible_v<");
      //Find the comma separating the two arguments
      size_t comma = requiresText.find(',', pos);
      if (comma == std::string::npos) return;
      //Replace the text between '<' and comma with std::decay_t<F>
      std::string mutated = requiresText.substr(0, pos) + "std::decay_t<" + ParamName + ">" + requiresText.substr(comma);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CR->getSourceRange()), mutated);
    }
}
  
void MutatorFrontendAction_120::MutatorASTConsumer_120::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(
      hasDescendant(expr().bind("RequiresExpr"))
    ).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}