//source file
#include "../include/Specialize_Template_Alias_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto aliassource = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << aliassource << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto temp_args = MT->getTypeSourceInfo();
      if (!temp_args)
        return;
      auto args =
          stringutils::rangetoStr(*(Result.SourceManager), temp_args->getTypeLoc().getSourceRange());
      llvm::outs() << args << '\n';
      auto temp_spec = MT->getTypeSourceInfo();
      auto temp_spec_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                   temp_spec->getTypeLoc().getSourceRange());
      llvm::outs() << temp_spec_str << '\n';
      auto temp_spec_name = temp_spec_str.substr(0, temp_spec_str.find('<'));
      llvm::outs() << temp_spec_name << '\n';
      string new_decl = "template <typename U> using " + temp_spec_name +
                        "= UnrelatedType<U>";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), new_decl);
    }
}
  
void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}