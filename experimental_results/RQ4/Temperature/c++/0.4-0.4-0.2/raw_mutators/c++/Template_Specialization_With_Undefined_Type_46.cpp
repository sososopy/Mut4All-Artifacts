//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_specialization_with_undefined_type_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/template_specialization_with_undefined_type_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Spec = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
      //Filter nodes in header files
      if (!Spec || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Spec->getLocation()))
        return;

      //Get the source code text of target node
      auto specialization = stringutils::rangetoStr(*(Result.SourceManager),
                                                    Spec->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedSpecialization = specialization;
      size_t pos = mutatedSpecialization.find('<');
      if (pos != std::string::npos) {
          mutatedSpecialization.replace(pos + 1, mutatedSpecialization.find('>') - pos - 1, "UndefinedType");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Spec->getSourceRange()), mutatedSpecialization);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}