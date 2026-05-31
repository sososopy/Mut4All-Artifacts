//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Nested_Template_Template_Parameter_In_Concept_438
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::TemplateDecl*> templateDecls;
        std::vector<const clang::ConceptDecl*> conceptDecls;
    };
};

//source file
#include "../include/Introduce_Nested_Template_Template_Parameter_In_Concept_438.h"

// ========================================================================================================
#define MUT438_OUTPUT 1

void MutatorFrontendAction_438::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      conceptDecls.push_back(CD);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(TD);
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("TargetConcept")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptText = stringutils::rangeToStr(*(Result.SourceManager), CD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedConcept;
      std::string conceptName = CD->getNameAsString();
      std::string templateParam = CD->getTemplateParameters()->getParam(0)->getNameAsString();
      
      // Find a suitable template template parameter from recorded templates
      std::string nestedTemplateName = "TT";
      if (!templateDecls.empty()) {
        size_t idx = getrandom::getRandomIndex(templateDecls.size() - 1);
        auto selectedTemplate = templateDecls[idx];
        nestedTemplateName = selectedTemplate->getNameAsString();
      }
      
      // Construct mutated concept with nested template template parameter requirement
      mutatedConcept = "template <class " + templateParam + ">\nconcept " + conceptName + " = requires {\n    template <template <class> class> struct void_t {};\n    typename void_t<" + templateParam + "::template nested>;\n};";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), mutatedConcept);
    }
}
  
void MutatorFrontendAction_438::MutatorASTConsumer_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher conceptMatcher = conceptDecl().bind("ConceptDecl");
    DeclarationMatcher templateMatcher = templateDecl().bind("TemplateDecl");
    DeclarationMatcher targetConceptMatcher = conceptDecl().bind("TargetConcept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(targetConceptMatcher, &callback);
    matchFinder.matchAST(Context);
}