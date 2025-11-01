//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Class_Inheritance_With_Concept_Constraints_136
 */ 
class MutatorFrontendAction_136 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(136)

private:
    class MutatorASTConsumer_136 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_136(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_template_class_inheritance_with_concept_constraints_136.h"

// ========================================================================================================
#define MUT136_OUTPUT 1

void MutatorFrontendAction_136::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      //Get the source code text of target node
      auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                  TD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string conceptConstraint = "template <typename T> concept IntegralOrFloating = std::is_integral_v<T> || std::is_floating_point_v<T>;";
      std::string requiresClause = "requires IntegralOrFloating<T>";
      size_t pos = templateDecl.find("template <typename T>");
      if (pos != std::string::npos) {
        templateDecl.insert(pos, conceptConstraint + "\n");
        pos = templateDecl.find("class");
        if (pos != std::string::npos) {
          templateDecl.insert(pos, requiresClause + " ");
        }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_136::MutatorASTConsumer_136::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(isDerivedFrom(hasType(cxxRecordDecl()))))).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}