//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Atomic_Constraint_In_Requires_Clause_438
 */ 
class MutatorFrontendAction_438 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(438)

private:
    class MutatorASTConsumer_438 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_438(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Atomic_Constraint_In_Requires_Clause_438.h"

// ========================================================================================================
#define MUT438_OUTPUT 1

void MutatorFrontendAction_438::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      if (cur_concepts.empty())
        return;
      auto idx = getrandom::getRandomIndex(cur_concepts.size() - 1);
      auto concept_name = cur_concepts[idx]->getNameAsString();
      llvm::SmallVector<const clang::Expr *> constraints;
      CT->getAssociatedConstraints(constraints);
      if (!constraints.empty()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               constraints.front()->getSourceRange());
        llvm::outs() << content << '\n';
        auto pos = content.find("requires");
        if (pos != string::npos)
          content.erase(pos);
        content = "/*mut438*/requires " + concept_name + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(constraints.front()->getSourceRange()),
            content);
      } else {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               CT->getSourceRange());
        content.insert(content.find('>') + 1, "/*mut438*/requires " +
                                                  concept_name + " ");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(CT->getSourceRange()), content);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (cur_concepts.empty())
        return;
      auto idx = getrandom::getRandomIndex(cur_concepts.size() - 1);
      auto concept_name = cur_concepts[idx]->getNameAsString();
      llvm::SmallVector<const clang::Expr *> constraints;
      FT->getAssociatedConstraints(constraints);
      if (!constraints.empty()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               constraints.front()->getSourceRange());
        llvm::outs() << content << '\n';
        auto pos = content.find("requires");
        if (pos != string::npos)
          content.erase(pos);
        content = "/*mut438*/requires " + concept_name + content;
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(constraints.front()->getSourceRange()),
            content);
      } else {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               FT->getSourceRange());
        content.insert(content.find('>') + 1, "/*mut438*/requires " +
                                                  concept_name + " ");
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FT->getSourceRange()), content);
      }
    } else if (auto *CP =
                   Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      if (!CP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CP->getLocation()))
        return;
      cur_concepts.push_back(CP);
    }
}
  
void MutatorFrontendAction_438::MutatorASTConsumer_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_template_matcher =
        classTemplateDecl().bind("ClassTemplate");
    auto function_template_matcher =
        functionTemplateDecl().bind("FunctionTemplate");
    auto concept_matcher = conceptDecl().bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.addMatcher(function_template_matcher, &callback);
    matchFinder.addMatcher(concept_matcher, &callback);
    matchFinder.matchAST(Context);
}