//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Constraint_With_Another_Concept_86
 */ 
class MutatorFrontendAction_86 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(86)

private:
    class MutatorASTConsumer_86 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_86(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ConceptDecl *> concepts;
    };
};

//source file
#include "../include/Replace_Template_Constraint_With_Another_Concept_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isTemplated() == false)
        return;
      if (MT->getDescribedTemplate() == nullptr)
        return;
      auto temdecl = MT->getDescribedTemplate();
      if (temdecl->getTemplateParameters() == nullptr)
        return;
      auto temparms = temdecl->getTemplateParameters();
      for (auto temparm : *temparms) {
        if (temparm->getKind() != Decl::Kind::TemplateTypeParm)
          continue;
        auto typarm = llvm::dyn_cast<TemplateTypeParmDecl>(temparm);
        if (typarm->hasTypeConstraint() == false)
          continue;
        auto tycon = typarm->getTypeConstraint();
        if (tycon == nullptr)
          continue;
        auto conname = tycon->getNamedConcept()->getNameAsString();
        if (conname == "")
          continue;
        llvm::outs() << conname << '\n';
        if (concepts.empty()) {
          auto newcon =
              "template <typename T> concept always_false = false;";
          Rewrite.ReplaceText(MT->getBeginLoc(), 0, "/*mut86*/" + (std::string)newcon);
          Rewrite.ReplaceText(tycon->getBeginLoc(), tycon->getNamedConcept()->getNameAsString().size(),
                              "always_false");
        } else {
          auto choice = getrandom::getRandomIndex(concepts.size() - 1);
          auto newcon = concepts[choice]->getNameAsString();
          llvm::outs() << newcon << '\n';
          Rewrite.ReplaceText(tycon->getBeginLoc(), tycon->getNamedConcept()->getNameAsString().size(),
                              newcon);
        }
      }
    } else if (auto *CD =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      concepts.push_back(CD);
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl().bind("Methods");
    auto concepts = clang::ast_matchers::conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(concepts, &callback);
    matchFinder.matchAST(Context);
}