//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_String_Literal_With_Function_Call_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)
private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableFunctions;
    };
};

//source file
#include "../include/Replace_String_Literal_With_Function_Call_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        QualType returnType = funcDecl->getReturnType();
        if (auto *specType = returnType->getAs<clang::ClassTemplateSpecializationType>()) {
            if (auto *templateDecl = specType->getTemplate()->getAsTemplateDecl()) {
                if (templateDecl->getQualifiedNameAsString() == "std::string") {
                    availableFunctions.push_back(funcDecl->getNameAsString());
                }
            }
        }
    } else if (auto *varDecl = Result.Nodes.getNodeAs<clang::VarDecl>("stringViewDecl")) {
        if (!availableFunctions.empty()) {
            if (auto *stringLiteral = Result.Nodes.getNodeAs<clang::StringLiteral>("stringLiteral")) {
                std::string funcCall = availableFunctions[0] + "()";
                funcCall = "/*mut34*/" + funcCall;
                SourceLocation loc = stringLiteral->getBeginLoc();
                SourceRange range = stringLiteral->getSourceRange();
                Rewrite.ReplaceText(range, funcCall);
            }
        }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcMatcher = functionDecl(
        returns(qualType(
            hasDeclaration(classTemplateSpecializationDecl(
                hasName("std::string")))))
    ).bind("funcDecl");
    DeclarationMatcher varMatcher = varDecl(
        hasType(classTemplateSpecializationDecl(
            hasName("std::string_view"))),
        hasInitializer(ignoringImplicit(implicitCastExpr(
            has(ignoringParenImpCasts(stringLiteral().bind("stringLiteral"))))))
    ).bind("stringViewDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}