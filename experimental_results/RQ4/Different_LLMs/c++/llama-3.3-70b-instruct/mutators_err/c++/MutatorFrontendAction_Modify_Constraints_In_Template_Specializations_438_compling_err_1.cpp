//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constraints_In_Template_Specializations_438
 */ 
class MutatorFrontendAction_Modify_Constraints_In_Template_Specializations_438 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Modify_Constraints_In_Template_Specializations_438)

private:
    class MutatorASTConsumer_Modify_Constraints_In_Template_Specializations_438 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Modify_Constraints_In_Template_Specializations_438(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Constraints_In_Template_Specializations_438.h"

// ========================================================================================================
#define MODIFY_CONSTRAINTS_IN_TEMPLATE_SPECIALIZATIONS_438_OUTPUT 1

void MutatorFrontendAction_Modify_Constraints_In_Template_Specializations_438::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TS->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // For example, replace "typename void_t<T::template value_types>" with "typename void_t<T::template other_types>"
      declaration = declaration.replace(declaration.find("value_types"), 11, "other_types");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_Modify_Constraints_In_Template_Specializations_438::MutatorASTConsumer_Modify_Constraints_In_Template_Specializations_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}