//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Inline_Assembly_And_Pragma_251
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
    };
};

//source file
#include "../include/Modify_Inline_Assembly_And_Pragma_251.h"

// ========================================================================================================
#define MUT251_OUTPUT 1

void MutatorFrontendAction_251::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ASM = Result.Nodes.getNodeAs<clang::AsmStmt>("asmStmt")) {
      //Filter nodes in header files
      if (!ASM || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ASM->getAsmLoc()))
        return;

      //Get the source code text of target node
      std::string asmText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    ASM->getSourceRange());

      // Modify the inline assembly code to reference an undefined label
      size_t labelPos = asmText.find(".Lfoo");
      if (labelPos != std::string::npos) {
        asmText.replace(labelPos, 5, ".Lundefined");
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ASM->getSourceRange()), asmText);
    } else if (auto *PPD = Result.Nodes.getNodeAs<clang::PragmaDirective>("preprocessorDirective")) {
      //Filter nodes in header files
      if (!PPD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PPD->getBeginLoc()))
        return;

      //Get the source code text of target node
      std::string pragmaText = stringutils::rangetoStr(*(Result.SourceManager),
                                                       PPD->getSourceRange());

      // Modify the pragma directive to suppress a different warning
      size_t warningPos = pragmaText.find("-Wmissing-noreturn");
      if (warningPos != std::string::npos) {
        pragmaText.replace(warningPos, 18, "-Wunused-label");
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), pragmaText);
    }
}
  
void MutatorFrontendAction_251::MutatorASTConsumer_251::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher asmMatcher = asmStmt().bind("asmStmt");
    DeclarationMatcher pragmaMatcher = pragmaDirective().bind("preprocessorDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(asmMatcher, &callback);
    matchFinder.addMatcher(pragmaMatcher, &callback);
    matchFinder.matchAST(Context);
}