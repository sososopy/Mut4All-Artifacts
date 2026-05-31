//header file
#pragma once
#include "Mutator_base.h"

/**
 * Bitfield_Width_with_Large_Integral_Constant_252
 */ 
class MutatorFrontendAction_252 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(252)

private:
    class MutatorASTConsumer_252 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_252(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Bitfield_Width_with_Large_Integral_Constant_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BF = Result.Nodes.getNodeAs<clang::FieldDecl>("bitfield")) {
      if (!BF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BF->getLocation()))
        return;
      if (!BF->isBitField())
        return;

      auto widthExpr = BF->getBitWidth();
      if (!widthExpr)
        return;

      auto widthStr = stringutils::rangetoStr(*(Result.SourceManager),
                                              widthExpr->getSourceRange());

      std::string largeWidthStr = "1 + (unsigned __int128)0xffffffffffffffff";
      llvm::outs() << "Original width: " << widthStr << ", Mutated width: "
                   << largeWidthStr << "\n";

      Rewrite.ReplaceText(widthExpr->getSourceRange(), largeWidthStr);
    }
}
  
void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl().bind("bitfield");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}