//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Undefined_Template_Instantiation_389
 */ 
class MutatorFrontendAction_389 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(389)

private:
    class MutatorASTConsumer_389 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_389(Rewriter &R) : TheRewriter(R) {}
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
        const clang::Decl *target_decl;
    };
};

//source file
#include "../include/Struct_Undefined_Template_Instantiation_389.h"

// ========================================================================================================
#define MUT389_OUTPUT 1

void MutatorFrontendAction_389::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isStruct())
        return;
      if (!MT->isCompleteDefinition())
        return;
      auto decls = MT->decls();
      for (auto decl : decls) {
        if (decl->getKind() == Decl::Kind::Field) {
          auto field = dyn_cast<FieldDecl>(decl);
          auto type = field->getType();
          if (type->isDependentType()) {
            target_decl = decl;
            llvm::outs() << "Find target\n";
          }
        }
      }
    }
    if (auto *DL = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL != target_decl)
        return;
      auto declstr = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      llvm::outs() << declstr << '\n';
      auto pos = declstr.find('<');
      if (pos != string::npos) {
        declstr.erase(declstr.begin() + pos, declstr.end());
      }
      llvm::outs() << declstr << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), declstr);
    }
}
  
void MutatorFrontendAction_389::MutatorASTConsumer_389::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Struct");
    auto field_matcher = fieldDecl().bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(field_matcher, &callback);
    matchFinder.matchAST(Context);
}