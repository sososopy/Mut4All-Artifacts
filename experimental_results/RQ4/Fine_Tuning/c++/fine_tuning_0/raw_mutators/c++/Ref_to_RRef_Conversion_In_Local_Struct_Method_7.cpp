//header file
#pragma once
#include "Mutator_base.h"

/**
 * Ref_to_RRef_Conversion_In_Local_Struct_Method_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Ref_to_RRef_Conversion_In_Local_Struct_Method_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("LocalStructMethod")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto body = MT->getBody();
      auto refs = body->getDescendants<DeclRefExpr>();
      for (auto ref : refs) {
        auto VD = ref.getDecl();
        if (VD->isLocalVarDeclOrParm()) {
          auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                              ref.getSourceRange());
          text = "std::move(" + text + ")";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(ref.getSourceRange()), text);
        }
      }
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
                                      hasParent(cxxRecordDecl(isStruct())),
                                      hasAncestor(functionDecl()))
                                      .bind("LocalStructMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}