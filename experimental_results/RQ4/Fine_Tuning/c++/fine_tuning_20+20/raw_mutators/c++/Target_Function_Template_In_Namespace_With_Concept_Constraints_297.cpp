//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Function_Template_In_Namespace_With_Concept_Constraints_297
 */ 
class MutatorFrontendAction_297 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(297)

private:
    class MutatorASTConsumer_297 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_297(Rewriter &R) : TheRewriter(R) {}
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
        const clang::NamespaceDecl *cur_ns;
    };
};

//source file
#include "../include/target_function_template_in_namespace_with_concept_constraints_297.h"

// ========================================================================================================
#define MUT297_OUTPUT 1

void MutatorFrontendAction_297::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      cur_ns = NS;
      llvm::outs() << "Namespace: " << cur_ns->getNameAsString() << '\n';
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (cur_ns == nullptr)
        return;
      if (FT->getDeclContext() != cur_ns)
        return;
      if (FT->getConstraints() == nullptr)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FT->getSourceRange());
      llvm::outs() << "Function Template: " << content << '\n';
      llvm::outs() << "Constraints: " << *(FT->getConstraints()) << '\n';
      auto pos = content.find("requires");
      if (pos == string::npos)
        return;
      content.replace(pos, 20, "requires std::same_as<fun>");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(FT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_297::MutatorASTConsumer_297::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto ns_matcher = namespaceDecl().bind("Namespace");
    auto ft_matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ns_matcher, &callback);
    matchFinder.addMatcher(ft_matcher, &callback);
    matchFinder.matchAST(Context);
}