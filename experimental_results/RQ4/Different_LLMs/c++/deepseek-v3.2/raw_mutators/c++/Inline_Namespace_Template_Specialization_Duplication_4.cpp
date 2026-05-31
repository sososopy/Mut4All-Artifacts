//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Template_Specialization_Duplication_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl*> inlineNamespaceTemplates;
        std::vector<const clang::FunctionDecl*> specializationsToDuplicate;
    };
};

//source file
#include "../include/Inline_Namespace_Template_Specialization_Duplication_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("inlineTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      
      // Check if this template is inside an inline namespace
      auto *NS = dyn_cast<clang::NamespaceDecl>(FT->getDeclContext());
      if (NS && NS->isInline()) {
        inlineNamespaceTemplates.push_back(FT);
      }
    }
    else if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("specialization")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      
      // Check if this is an explicit specialization
      if (FS->isTemplateSpecialization() && FS->getTemplateSpecializationKind() == clang::TSK_ExplicitSpecialization) {
        // Check if it specializes a template from our collected inline namespace templates
        auto *FT = FS->getPrimaryTemplate();
        if (FT && std::find(inlineNamespaceTemplates.begin(), inlineNamespaceTemplates.end(), FT) != inlineNamespaceTemplates.end()) {
          specializationsToDuplicate.push_back(FS);
        }
      }
    }
    else if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("duplicateTarget")) {
      // This is where we actually perform the duplication
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      
      // Check if this specialization is in our list to duplicate
      if (std::find(specializationsToDuplicate.begin(), specializationsToDuplicate.end(), FS) != specializationsToDuplicate.end()) {
        //Get the source code text of target node
        auto specializationText = stringutils::rangetoStr(*(Result.SourceManager), FS->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        // Duplicate the specialization by inserting it right after the original
        std::string duplicateText = "\n" + specializationText + " // Duplicated specialization";
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(FS->getEndLoc(), duplicateText);
        
        // Remove from list to avoid duplicate mutations
        specializationsToDuplicate.erase(std::remove(specializationsToDuplicate.begin(), specializationsToDuplicate.end(), FS), specializationsToDuplicate.end());
      }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First find all function templates in inline namespaces
    DeclarationMatcher inlineTemplateMatcher = functionTemplateDecl(hasAncestor(namespaceDecl(isInline()))).bind("inlineTemplate");
    
    // Then find explicit specializations
    DeclarationMatcher specializationMatcher = functionDecl(isExplicitTemplateSpecialization()).bind("specialization");
    
    // Finally match the specializations we want to duplicate
    DeclarationMatcher duplicateMatcher = functionDecl(isExplicitTemplateSpecialization()).bind("duplicateTarget");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inlineTemplateMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.addMatcher(duplicateMatcher, &callback);
    matchFinder.matchAST(Context);
}