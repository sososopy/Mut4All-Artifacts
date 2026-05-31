//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Unused_Concept_In_Template_Declaration_222
 */ 
class MutatorFrontendAction_222 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(222)

private:
    class MutatorASTConsumer_222 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_222(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Unused_Concept_In_Template_Declaration_222.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getKind() != Decl::Kind::ClassTemplate &&
          DL->getKind() != Decl::Kind::FunctionTemplate &&
          DL->getKind() != Decl::Kind::TypeAliasTemplate)
        return;
      auto params = DL->getTemplateParameters();
      for (auto param : *params) {
        if (param->getKind() == Decl::Kind::TemplateTypeParm) {
          auto TTP = dyn_cast<TemplateTypeParmDecl>(param);
          if (TTP->hasTypeConstraint()) {
            auto constraint = TTP->getTypeConstraint();
            auto constraint_type = constraint->getNamedConcept();
            auto concept_name = constraint_type->getName();
            llvm::outs() << concept_name << '\n';
            auto DL_range = DL->getSourceRange();
            auto DL_str = stringutils::rangetoStr(
                *(Result.SourceManager), DL->getSourceRange());
            DL_str.insert(DL_str.find(';'), "<" + concept_name.str() + ">");
            DL_str.insert(DL_str.rfind(';'), "(" + concept_name.str() + ")");
            DL_str = "/*mut222*/" + DL_str;
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(DL->getSourceRange()), DL_str);
          }
        }
      }
    }
  }

void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}