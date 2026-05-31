//source file
#include "../include/insert_unused_template_parameter_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isThisDeclarationADefinition())
        return;

      //Get the source code text of target node
      auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string templateParam = "template<typename UnusedParam>\n";
      functionText.insert(FD->getBeginLoc().getRawEncoding(), templateParam);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}