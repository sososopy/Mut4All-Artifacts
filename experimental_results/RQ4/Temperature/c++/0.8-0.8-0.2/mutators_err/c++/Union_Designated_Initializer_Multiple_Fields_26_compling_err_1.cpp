//header file
#pragma once
#include "Mutator_base.h"

/**
 * union_designated_initializer_multiple_fields_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/union_designated_initializer_multiple_fields_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::UnionDecl>("unions")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        std::string unionName = UD->getNameAsString();
        if (unionName.empty())
            return;

        if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
            if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
                return;

            std::string varInit = "{ .a = 10, .b = 20.5 }";
            std::string newInit = "/*mut26*/" + unionName + " " + VD->getNameAsString() + " = " + varInit + ";";
            Rewrite.ReplaceText(VD->getSourceRange(), newInit);
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher unionMatcher = unionDecl(has(fieldDecl(hasType(builtinType()).bind("fields")), fieldDecl(hasType(namedDecl())).bind("fields"))).bind("unions");
    DeclarationMatcher varMatcher = varDecl(hasType(recordDecl(hasName("UnionType")))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}