//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_specialization_constraint_649
 */ 
class MutatorFrontendAction_649 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(649)

private:
    class MutatorASTConsumer_649 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_649(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_specialization_constraint_649.h"

// ========================================================================================================
#define MUT649_OUTPUT 1

void MutatorFrontendAction_649::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string conflictingSpecialization = "template<typename U> requires (!B<U>)\nvoid g();\n";
      declaration.insert(declaration.rfind('}'), "\n/*mut649*/" + conflictingSpecialization);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_649::MutatorASTConsumer_649::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasAnyTemplateArgument(refersToType(hasDeclaration(namedDecl(hasName("g"))))),
        hasDescendant(conceptSpecializationExpr())
    ).bind("FunctionTemplate");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}