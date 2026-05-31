//source file
#include "../include/Mutate_Attribute_Index_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = stringutils::rangetoStr(*(Result.SourceManager),
                                          FD->getSourceRange());
      llvm::outs() << decl << '\n';
      if (FD->hasAttr<FormatAttr>()) {
        auto Attr = FD->getAttr<FormatAttr>();
        auto idx = Attr->getFormatIdx();
        auto idx2 = Attr->getFirstArg();
        llvm::outs() << idx << ' ' << idx2 << '\n';
        if (FD->isVariadic()) {
          auto param = FD->parameters();
          int param_size = param.size();
          if (param_size == 0)
            return;
          int dice = getrandom::getRandomIndex(param_size - 1);
          if (dice == 0)
            dice = param_size;
          else
            dice = param_size + getrandom::getRandomIndex(3);
          llvm::outs() << dice << '\n';
          auto target = "__attribute__((format(printf, " +
                        std::to_string(dice) + ", " + std::to_string(dice + 1) +
                        ")))";
          llvm::outs() << target << '\n';
          //Perform mutation on the source code text by applying string replacement
          stringutils::strReplace(decl, "__attribute__((format(printf, " +
                                            std::to_string(idx) + ", " +
                                            std::to_string(idx2) + ")))",
                                  target);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(FD->getSourceRange()), decl);
        }
      }
    }
}
  
void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}