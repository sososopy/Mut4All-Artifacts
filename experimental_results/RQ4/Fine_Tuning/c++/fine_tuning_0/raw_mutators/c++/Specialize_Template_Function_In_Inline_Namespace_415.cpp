//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_Function_In_Inline_Namespace_415
 */ 
class MutatorFrontendAction_415 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(415)

private:
    class MutatorASTConsumer_415 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_415(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamespaceDecl *> inline_namespaces;
        std::vector<const clang::FunctionTemplateDecl *> template_functions;
    };
};

//source file
#include "../include/Specialize_Template_Function_In_Inline_Namespace_415.h"

// ========================================================================================================
#define MUT415_OUTPUT 1

void MutatorFrontendAction_415::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNamespace")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      if (NS->isInline()) {
        inline_namespaces.push_back(NS);
      }
    } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "TemplateFunction")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      template_functions.push_back(FT);
    } else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "TranslationUnit")) {
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;
      if (inline_namespaces.empty())
        return;
      if (template_functions.empty())
        return;
      //Get the source code text of target node
      int index = getrandom::getRandomIndex(template_functions.size() - 1);
      auto target_template_function = template_functions[index];
      auto target_function_name = target_template_function->getNameAsString();
      auto target_namespace = inline_namespaces[0];
      auto target_namespace_name = target_namespace->getNameAsString();
      auto specialization =
          "template<> void " + target_function_name + "<int>();";
      llvm::outs() << specialization << '\n';
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(target_namespace->getEndLoc(), 0, specialization);
    }
}
  
void MutatorFrontendAction_415::MutatorASTConsumer_415::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto inline_namespace_matcher = namespaceDecl().bind("InlineNamespace");
    auto template_function_matcher = functionTemplateDecl().bind("TemplateFunction");
    auto translation_unit_matcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inline_namespace_matcher, &callback);
    matchFinder.addMatcher(template_function_matcher, &callback);
    matchFinder.addMatcher(translation_unit_matcher, &callback);
    matchFinder.matchAST(Context);
}