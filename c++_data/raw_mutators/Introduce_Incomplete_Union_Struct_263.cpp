//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Union_Struct_263
 */ 
class MutatorFrontendAction_263 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(263)

private:
    class MutatorASTConsumer_263 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_263(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_incomplete_union_struct_263.h"

// ========================================================================================================
#define MUT263_OUTPUT 1

void MutatorFrontendAction_263::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("unionDecl")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      if (UD->isUnion() && UD->isCompleteDefinition()) {
        auto unionSource = stringutils::rangetoStr(*(Result.SourceManager), UD->getSourceRange());
        std::string mutation = "struct { ; }; typedef ";
        auto pos = unionSource.find_last_of('}');
        if (pos != std::string::npos) {
          unionSource.insert(pos, mutation);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(UD->getSourceRange()), unionSource);
        }
      }
    }
}
  
void MutatorFrontendAction_263::MutatorASTConsumer_263::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isUnion()).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}