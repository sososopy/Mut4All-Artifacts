//source file
#include "../include/Conditional_Operator_Mutation_For_Auto_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("AutoVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto origin = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      llvm::outs() << "origin: " << origin << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (origin.find('?') == string::npos)
        return;
      auto cond = origin.substr(origin.find('?') + 1);
      llvm::outs() << "cond: " << cond << '\n';
      auto left = cond.substr(0, cond.find(':'));
      auto right = cond.substr(cond.find(':') + 1);
      llvm::outs() << "left: " << left << " right: " << right << '\n';
      if (left.find('{') != string::npos && right.find('{') == string::npos) {
        left = left.substr(left.find('{') + 1, left.find('}') - 1);
        llvm::outs() << "left: " << left << '\n';
        right = right.substr(0, right.find('['));
        right = right + "[0]";
        llvm::outs() << "right: " << right << '\n';
        origin = origin.substr(0, origin.find('?')) + "?" + left + ":" + right;
        llvm::outs() << "mutated: " << origin << '\n';
      } else if (right.find('{') != string::npos &&
                 left.find('{') == string::npos) {
        right = right.substr(right.find('{') + 1, right.find('}') - 1);
        llvm::outs() << "right: " << right << '\n';
        left = left.substr(0, left.find('['));
        left = left + "[0]";
        llvm::outs() << "left: " << left << '\n';
        origin = origin.substr(0, origin.find('?')) + "?" + left + ":" + right;
        llvm::outs() << "mutated: " << origin << '\n';
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), origin);
    }
}
  
void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(autoType())).bind("AutoVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}