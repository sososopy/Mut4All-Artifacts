//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_For_Result_Type_With_Lambda_Capture_16
 */ 

class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> structs;
    };
};

//source file
#include "../include/Template_Specialization_For_Result_Type_With_Lambda_Capture_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAD = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("typeAlias")) {
        if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(TAD->getLocation()))
            return;

        const clang::Type *underlyingType = TAD->getUnderlyingType().getTypePtr();
        if (const auto *DT = dyn_cast<clang::DecltypeType>(underlyingType)) {
            const clang::Expr *underlyingExpr = DT->getUnderlyingExpr();
            if (const auto *CE = dyn_cast<clang::CXXConstructExpr>(underlyingExpr)) {
                const clang::CXXConstructorDecl *ctor = CE->getConstructor();
                if (ctor && ctor->getDeclName().getQualifiedNameAsString() == "std::function::function") {
                    if (CE->getNumArgs() >= 1) {
                        const clang::Expr *arg = CE->getArg(0);
                        if (const auto *LE = dyn_cast<clang::LambdaExpr>(arg)) {
                            if (LE->hasCaptures()) {
                                const clang::LambdaCapture *capture = LE->capture_begin();
                                const clang::VarDecl *capturedVar = capture->getCapturedVar();
                                if (!capturedVar)
                                    return;

                                std::string capturedVarName = capturedVar->getNameAsString();
                                if (!structs.empty()) {
                                    const clang::CXXRecordDecl *structDecl = structs[0];
                                    std::string structName = structDecl->getNameAsString();
                                    std::string typeAliasName = TAD->getNameAsString();

                                    std::string code = "template<> auto " + typeAliasName + "::operator()(" + structName + " a) { return " + capturedVarName + "; }\n";
                                    code = "/*mut16*/" + code;

                                    SourceLocation endLoc = TAD->getEndLoc();
                                    Rewrite.InsertTextAfter(endLoc, code);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("struct")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        if (CD->isLambda())
            return;
        if (!CD->isCompleteDefinition())
            return;

        structs.push_back(CD);
    }
}

void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    DeclarationMatcher typeAliasMatcher = 
        typeAliasDecl(
            hasType(decltypeType(
                hasUnderlyingExpr(
                    cxxConstructExpr(
                        hasDeclaration(cxxConstructorDecl(hasName("std::function::function"))),
                        hasArgument(0, lambdaExpr())
                    )
                )
            )
        ).bind("typeAlias");

    DeclarationMatcher structMatcher = 
        cxxRecordDecl(
            unless(isLambda()),
            hasDefinition()
        ).bind("struct");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeAliasMatcher, &callback);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.matchAST(Context);
}