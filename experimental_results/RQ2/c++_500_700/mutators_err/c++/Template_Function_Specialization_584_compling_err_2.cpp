//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_function_specialization_584
 */ 
class MutatorFrontendAction_584 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(584)

private:
    class MutatorASTConsumer_584 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_584(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionTemplateDecl *OriginalTemplateDecl = nullptr;
        const FunctionDecl *SpecializationDecl = nullptr;
    };
};

//source file
#include "../include/Template_Function_Specialization_584.h"

// ========================================================================================================
#define MUT584_OUTPUT 1

void MutatorFrontendAction_584::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      // Record the original template function
      OriginalTemplateDecl = FTD;

      // Iterate over specializations and introduce a new one if not present
      for (auto *Spec : FTD->specializations()) {
        if (Spec->getTemplateSpecializationArgs()->size() == 1) {
          if (auto Arg = Spec->getTemplateSpecializationArgs()->get(0).getAsType()) {
            if (Arg->isSpecificBuiltinType(BuiltinType::Int)) {
              SpecializationDecl = Spec;
              break;
            }
          }
        }
      }

      if (SpecializationDecl) {
        // Get the source code text of target node
        auto originalSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                      SpecializationDecl->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string newSpecialization = "template<> void " + OriginalTemplateDecl->getNameAsString() +
                                        "<const int>(const int value) {\n" +
                                        "    // Slightly different logic for const int type\n" +
                                        "}\n";

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfter(SpecializationDecl->getEndLoc(), "\n/*mut584*/" + newSpecialization);
      }
    }
}
  
void MutatorFrontendAction_584::MutatorASTConsumer_584::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}