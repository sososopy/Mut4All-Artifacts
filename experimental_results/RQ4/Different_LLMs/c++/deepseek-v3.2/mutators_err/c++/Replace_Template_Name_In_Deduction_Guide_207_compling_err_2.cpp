//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Name_In_Deduction_Guide_207
 */ 
class MutatorFrontendAction_207 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(207)

private:
    class MutatorASTConsumer_207 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_207(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamedDecl*> templatesInOtherNamespace;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT207_OUTPUT 1

void MutatorFrontendAction_207::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DG = Result.Nodes.getNodeAs<clang::CXXDeductionGuideDecl>("DeductionGuide")) {
      //Filter nodes in header files
      if (!DG || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DG->getLocation()))
        return;
      //Get the source code text of target node
      auto deductionGuideText = stringutils::rangetoStr(*(Result.SourceManager), DG->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      auto templateName = DG->getDeducedTemplate()->getNameAsString();
      //Find a template with same unqualified name in a different namespace
      const clang::NamedDecl* candidateTemplate = nullptr;
      for (auto* decl : templatesInOtherNamespace) {
          if (decl->getNameAsString() == templateName && decl->getDeclContext() != DG->getDeducedTemplate()->getDeclContext()) {
              candidateTemplate = decl;
              break;
          }
      }
      if (!candidateTemplate) return;
      //Perform mutation on the source code text by applying string replacement
      std::string namespaceQualifier = "";
      if (const clang::NamespaceDecl* ns = clang::dyn_cast<clang::NamespaceDecl>(candidateTemplate->getDeclContext())) {
          namespaceQualifier = ns->getNameAsString() + "::";
      } else if (const clang::RecordDecl* rd = clang::dyn_cast<clang::RecordDecl>(candidateTemplate->getDeclContext())) {
          namespaceQualifier = rd->getNameAsString() + "::";
      } else {
          return;
      }
      //Replace unqualified name with qualified name in deduction guide declaration and trailing return type
      std::string mutatedText = deductionGuideText;
      size_t pos = mutatedText.find(templateName);
      while (pos != std::string::npos) {
          mutatedText.replace(pos, templateName.length(), namespaceQualifier + templateName);
          pos = mutatedText.find(templateName, pos + namespaceQualifier.length() + templateName.length());
      }
      mutatedText = "/*mut207*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DG->getSourceRange()), mutatedText);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::NamedDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record template declarations for later use
      templatesInOtherNamespace.push_back(TD);
    }
}
  
void MutatorFrontendAction_207::MutatorASTConsumer_207::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher deductionGuideMatcher = cxxDeductionGuideDecl().bind("DeductionGuide");
    DeclarationMatcher templateMatcher = namedDecl(anyOf(
        classTemplateDecl(),
        templateTypeParmDecl(),
        nonTypeTemplateParmDecl()
    )).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(deductionGuideMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}