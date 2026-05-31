//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Template_Concept_103
 */ 
class MutatorFrontendAction_103 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(103)

private:
    class MutatorASTConsumer_103 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_103(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> cur_concepts;
    };
};

//source file
#include "../include/replace_template_template_concept_103.h"

// ========================================================================================================
#define MUT103_OUTPUT 1

void MutatorFrontendAction_103::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      if (TP->hasTypeConstraint() == false)
        return;
      auto constraint = TP->getTypeConstraint();
      auto type = constraint.getNamedConcept();
      if (type == nullptr)
        return;
      llvm::outs() << "mut103: " << type->getNameAsString() << '\n';
      string replacement = "template<" + type->getNameAsString() + '>';
      Rewrite.ReplaceText(TP->getSourceRange(), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      cur_concepts.push_back(DL);
    } else if (auto *TV = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>(
                   "TemplateTemplateParm")) {
      if (!TV || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TV->getLocation()))
        return;
      if (TV->hasTypeConstraint() == false)
        return;
      auto constraint = TV->getTypeConstraint();
      auto type = constraint.getNamedConcept();
      if (type == nullptr)
        return;
      llvm::outs() << "mut103: " << type->getNameAsString() << '\n';
      string replacement = "template<" + type->getNameAsString() + '>';
      Rewrite.ReplaceText(TV->getSourceRange(), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Var")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isConstrainedAuto())
        return;
      auto type = DL->getType();
      if (type->getAs<AutoType>() == nullptr)
        return;
      llvm::outs() << "mut103: " << DL->getNameAsString() << '\n';
      string replacement = "auto";
      for (auto concept : cur_concepts) {
        if (DL->getType().getAsString().find(concept->getNameAsString()) !=
            string::npos) {
          replacement += " requires " + concept->getNameAsString();
          break;
        }
      }
      Rewrite.ReplaceText(DL->getTypeSourceInfo()->getTypeLoc().getSourceRange(),
                          replacement);
    }
}
  
void MutatorFrontendAction_103::MutatorASTConsumer_103::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateTypeParmDecl().bind("TemplateTypeParm");
    auto concept_matcher = conceptDecl().bind("Concept");
    auto template_template_matcher =
        templateTemplateParmDecl().bind("TemplateTemplateParm");
    auto var_matcher = varDecl().bind("Var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.addMatcher(template_template_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}