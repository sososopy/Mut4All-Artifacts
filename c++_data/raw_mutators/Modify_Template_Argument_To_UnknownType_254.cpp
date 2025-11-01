//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_To_UnknownType_254
 */ 
class MutatorFrontendAction_254 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(254)

private:
    class MutatorASTConsumer_254 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_254(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_argument_to_unknown_type_254.h"

// ========================================================================================================
#define MUT254_OUTPUT 1

void MutatorFrontendAction_254::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("TypeY");
      if (pos != std::string::npos) {
          declaration.replace(pos, 5, "UnknownType");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_254::MutatorASTConsumer_254::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
        hasParameter(0, hasType(
            templateSpecializationType(
                hasTemplateArgument(1, refersToType(
                    templateSpecializationType(
                        hasTemplateArgument(0, refersToType(
                            qualType(hasDeclaration(namedDecl(hasName("TypeY"))))
                        ))
                    ))
                ))
            ))
        ).bind("FunctionWithTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}