//source file
#include "../include/Incomplete_Struct_Declaration_In_Template_363.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TC = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!TC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TC->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 TC->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutation = "\n/*mut363*/struct : " + TC->getNameAsString() + "<T> {} instance";
      if (declaration.rfind('}') != std::string::npos)
        declaration.insert(declaration.rfind('}'), mutation);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TC->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}