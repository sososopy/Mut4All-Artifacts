//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)

private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_167.h"

// ========================================================================================================
#define MUT167_OUTPUT 1

void MutatorFrontendAction_167::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;

      if (FS->hasBody()) {
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FS->getBody()->getSourceRange());
        
        std::string unreachableBlock = "\n/*mut167*/if (false) {\n";
        unreachableBlock += "  int unreachable_var = 0;\n";
        unreachableBlock += "  unreachable_var++;\n";
        unreachableBlock += "}\n";
        
        functionBody.insert(functionBody.find("{") + 1, unreachableBlock);
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_167::MutatorASTConsumer_167::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}