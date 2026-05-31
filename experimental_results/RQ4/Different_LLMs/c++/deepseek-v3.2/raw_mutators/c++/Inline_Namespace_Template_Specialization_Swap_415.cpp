//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Template_Specialization_Swap_415
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
        std::vector<const clang::FunctionDecl*> inlineNamespaceTemplates;
        std::vector<const clang::FunctionDecl*> externalSpecializations;
    };
};

//source file
#include "../include/Mutator_Inline_Namespace_Template_Specialization_Swap_415.h"

// ========================================================================================================
#define MUT415_OUTPUT 1

void MutatorFrontendAction_415::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("InlineNamespaceTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      if (FD->isTemplateInstantiation() && FD->getTemplatedKind() == clang::FunctionDecl::TK_FunctionTemplate) {
        const clang::DeclContext* DC = FD->getDeclContext();
        if (DC && DC->isInlineNamespace()) {
          inlineNamespaceTemplates.push_back(FD);
        }
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ExternalSpecialization")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      if (FD->isTemplateInstantiation() && FD->getTemplatedKind() == clang::FunctionDecl::TK_FunctionTemplateSpecialization) {
        const clang::DeclContext* DC = FD->getDeclContext();
        if (DC && !DC->isInlineNamespace()) {
          externalSpecializations.push_back(FD);
        }
      }
    }
    else if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNamespace")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      //Get the source code text of target node
      if (!NS->isInline()) return;
      auto namespaceRange = NS->getSourceRange();
      auto namespaceText = stringutils::rangetoStr(*Result.SourceManager, namespaceRange);
      //Perform mutation on the source code text by applying string replacement
      for (const auto* spec : externalSpecializations) {
        for (const auto* templ : inlineNamespaceTemplates) {
          if (spec->getInstantiatedFrom() == templ->getDescribedFunctionTemplate()) {
            auto specRange = spec->getSourceRange();
            auto specText = stringutils::rangetoStr(*Result.SourceManager, specRange);
            // Check if specialization is already inside namespace
            if (namespaceRange.getBegin().getRawEncoding() < specRange.getBegin().getRawEncoding() &&
                namespaceRange.getEnd().getRawEncoding() > specRange.getEnd().getRawEncoding()) {
              continue;
            }
            // Remove specialization from outside
            Rewrite.RemoveText(specRange);
            // Insert specialization inside namespace before closing brace
            size_t bracePos = namespaceText.rfind('}');
            if (bracePos != std::string::npos) {
              std::string newSpecText = "\n" + specText;
              Rewrite.InsertText(namespaceRange.getBegin().getLocWithOffset(bracePos), newSpecText);
            }
            // Only apply one mutation per namespace
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_415::MutatorASTConsumer_415::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher inlineTemplateMatcher = functionDecl(isTemplateInstantiation(), hasParent(namespaceDecl(isInline()))).bind("InlineNamespaceTemplate");
    DeclarationMatcher externalSpecMatcher = functionDecl(isTemplateInstantiation(), unless(hasParent(namespaceDecl(isInline())))).bind("ExternalSpecialization");
    DeclarationMatcher namespaceMatcher = namespaceDecl(isInline()).bind("InlineNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inlineTemplateMatcher, &callback);
    matchFinder.addMatcher(externalSpecMatcher, &callback);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.matchAST(Context);
}