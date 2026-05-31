//header file
#pragma once
#include "Mutator_base.h"

/**
 * Explicit_Instantiation_Constrained_Member_Overload_95
 */ 
class MutatorFrontendAction_95 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(95)

private:
    class MutatorASTConsumer_95 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_95(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> candidateOverloads;
    };
};

//source file
#include "../include/Mutator_Explicit_Instantiation_Constrained_Member_Overload_95.h"

// ========================================================================================================
#define MUT95_OUTPUT 1

void MutatorFrontendAction_95::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("explicitInst")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isExplicitlyInstantiated())
        return;
      if (!FD->isCXXClassMember())
        return;
      const clang::CXXRecordDecl* parentClass = FD->getParent()->getDescribedClassTemplate();
      if (!parentClass)
        return;
      const clang::ClassTemplateDecl* classTemplate = parentClass->getDescribedClassTemplate();
      if (!classTemplate)
        return;
      candidateOverloads.clear();
      for (auto* decl : classTemplate->getTemplatedDecl()->decls()) {
        if (auto* method = llvm::dyn_cast<clang::CXXMethodDecl>(decl)) {
          if (method->getName() == FD->getName()) {
            candidateOverloads.push_back(method);
          }
        }
      }
      if (candidateOverloads.size() < 2)
        return;
      bool hasConstraint = false;
      bool hasNoConstraint = false;
      for (auto* overload : candidateOverloads) {
        if (overload->getTrailingRequiresClause())
          hasConstraint = true;
        else
          hasNoConstraint = true;
      }
      if (!hasConstraint || !hasNoConstraint)
        return;
      //Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      std::string mutatedText;
      bool originalHasRequires = FD->getTrailingRequiresClause() != nullptr;
      if (originalHasRequires) {
        size_t requiresPos = originalText.find("requires");
        if (requiresPos != std::string::npos) {
          size_t endPos = originalText.find(';', requiresPos);
          if (endPos != std::string::npos) {
            mutatedText = originalText.substr(0, requiresPos) + originalText.substr(endPos);
          } else {
            mutatedText = originalText.substr(0, requiresPos) + ";";
          }
        } else {
          mutatedText = originalText;
        }
      } else {
        std::vector<const clang::FunctionDecl*> constrainedOverloads;
        for (auto* overload : candidateOverloads) {
          if (overload->getTrailingRequiresClause())
            constrainedOverloads.push_back(overload);
        }
        if (constrainedOverloads.empty())
          return;
        size_t index = getrandom::getRandomIndex(constrainedOverloads.size() - 1);
        auto* selectedOverload = constrainedOverloads[index];
        auto* requiresClause = selectedOverload->getTrailingRequiresClause();
        if (!requiresClause)
          return;
        std::string requiresText = stringutils::rangetoStr(*(Result.SourceManager), requiresClause->getSourceRange());
        size_t insertPos = originalText.find(')');
        if (insertPos != std::string::npos) {
          mutatedText = originalText.substr(0, insertPos + 1) + " requires " + requiresText + originalText.substr(insertPos + 1);
        } else {
          mutatedText = originalText;
        }
      }
      mutatedText = "/*mut95*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_95::MutatorASTConsumer_95::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isExplicitInstantiation(), isCXXClassMember()).bind("explicitInst");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}