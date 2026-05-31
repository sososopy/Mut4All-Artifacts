//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471
 */ 
class MutatorFrontendAction_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471)
private:
    class MutatorASTConsumer_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "stringutils.h"

// ========================================================================================================
#define MUT_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471_OUTPUT 1

void MutatorFrontendAction_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (auto *TParam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("tparam")) {
            if (auto *PVD = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
                // Ensure the parameter is part of this function
                if (FD->getParamDecl(PVD->getParameterIndex()) != PVD)
                    return;

                // Get the name of the variadic template parameter
                std::string tparam_name = TParam->getNameAsString();

                // Generate the fold expression as the default argument
                std::string default_arg = " = (std::is_arithmetic<" + tparam_name + ">::value && ...)";

                // Get the original parameter source code
                std::string param_code = stringutils::rangetoStr(*Result.SourceManager, PVD->getSourceRange());

                // Append the default argument to the parameter
                std::string new_param_code = param_code + default_arg;

                // Replace the original parameter with the mutated one
                Rewrite.ReplaceText(PVD->getSourceRange(), new_param_code);
            }
        }
    }
}
  
void MutatorFrontendAction_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471::MutatorASTConsumer_Replace_Function_Template_Parameter_With_Default_Argument_Using_Fold_Expression_471::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match variadic function templates with a parameter without a default argument
    DeclarationMatcher matcher = functionDecl(
        hasTemplateParameterList(
            templateParameterList(
                hasParameterPack(templateTypeParmDecl().bind("tparam"))
            )
        ),
        hasDescendant(
            parmVarDecl(
                hasNoDefaultArg()
            ).bind("param")
        )
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}