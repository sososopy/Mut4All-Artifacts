//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Attribute_Index_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(321)

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

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
        auto idx2 = Attr->getFirstArgIdx();
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