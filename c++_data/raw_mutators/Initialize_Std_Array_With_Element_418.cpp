//header file
#pragma once
#include "Mutator_base.h"

/**
 * Initialize_Std_Array_With_Element_418
 */ 
class MutatorFrontendAction_418 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(418)

private:
    class MutatorASTConsumer_418 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_418(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *AT = VD->getType()->getAs<clang::TemplateSpecializationType>()) {
        if (AT->getTemplateName().getAsTemplateDecl()->getNameAsString() == "array") {
          const clang::TemplateArgument &arg = AT->getArg(0);
          if (arg.getKind() == clang::TemplateArgument::Type) {
            clang::QualType elemType = arg.getAsType();
            std::string elemTypeStr = elemType.getAsString();
            std::string varName = VD->getNameAsString();
            std::string initializer = "{ " + elemTypeStr + " instance; " + varName + " = {instance}; }";
            
            //Perform mutation on the source code text by applying string replacement
            Rewrite.ReplaceText(VD->getSourceRange(), initializer);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType(hasDeclaration(namedDecl(hasName("std::array")))))).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}