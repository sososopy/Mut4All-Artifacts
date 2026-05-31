//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_251
 */ 
class MutatorFrontendAction_251 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(251)

private:
    class MutatorASTConsumer_251 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_251(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_251.h"

// ========================================================================================================
#define MUT251_OUTPUT 1

void MutatorFrontendAction_251::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::GCCAsmStmt>("InlineAsm")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getAsmLoc()))
        return;
      //Get the source code text of target node
      auto asm_str = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << asm_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto pos = asm_str.find(".L");
      if (pos == string::npos)
        return;
      auto pos2 = asm_str.find(":", pos);
      if (pos2 == string::npos)
        return;
      auto label = asm_str.substr(pos, pos2 - pos);
      llvm::outs() << label << '\n';
      asm_str = stringutils::strReplace(asm_str, label, ".Lnonexistent");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), asm_str);
    }
}
  
void MutatorFrontendAction_251::MutatorASTConsumer_251::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = asmStmt().bind("InlineAsm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}