//header file
#pragma once
#include "Mutator_base.h"

/**
 * misaligned_struct_declaration_567
 */ 
class MutatorFrontendAction_567 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(567)

private:
    class MutatorASTConsumer_567 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_567(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misaligned_struct_declaration_567.h"

// ========================================================================================================
#define MUT567_OUTPUT 1

void MutatorFrontendAction_567::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("unionStruct")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      
      if (UD->isAnonymousStructOrUnion() && UD->isStruct()) {
        for (auto *Field : UD->fields()) {
          auto fieldType = Field->getType().getAsString();
          if (fieldType == "int" || fieldType == "unsigned") {
            std::string newType = "igned";
            Rewrite.ReplaceText(Field->getTypeSourceInfo()->getTypeLoc().getSourceRange(), newType);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_567::MutatorASTConsumer_567::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isInUnion(), isStruct()).bind("unionStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}