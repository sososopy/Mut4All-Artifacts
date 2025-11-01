//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Struct_Attributes_434
 */ 
class MutatorFrontendAction_434 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(434)

private:
    class MutatorASTConsumer_434 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_434(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_434.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_434::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      if (SD->hasAttrs()) {
        auto &SM = *Result.SourceManager;
        auto startLoc = SD->getBeginLoc();
        auto endLoc = SD->getLocation();
        // Remove attributes from the struct
        for (const auto *Attr : SD->attrs()) {
          auto attrRange = Attr->getRange();
          Rewrite.RemoveText(attrRange);
        }
        // Ensure struct remains valid
        if (startLoc.isValid() && endLoc.isValid()) {
          auto structText = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(startLoc, endLoc));
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), structText);
        }
      }
    }
}
  
void MutatorFrontendAction_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}