//source file
#include "../include/Replace_Concept_With_Template_And_Requires_Clause_103.h"

// ========================================================================================================
#define MUT103_OUTPUT 1

void MutatorFrontendAction_103::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto param_str = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << param_str << '\n';
      auto loc = param_str.find("auto");
      auto concept_str = param_str.substr(0, loc);
      param_str = param_str.substr(loc);
      llvm::outs() << concept_str << '\n';
      llvm::outs() << param_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto mutated_param_str = "/*mut103*/template <typename T> requires " + concept_str + "<T> " + param_str;
      llvm::outs() << mutated_param_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutated_param_str);
    }
}
  
void MutatorFrontendAction_103::MutatorASTConsumer_103::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = parmVarDecl(hasType(autoType())).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}