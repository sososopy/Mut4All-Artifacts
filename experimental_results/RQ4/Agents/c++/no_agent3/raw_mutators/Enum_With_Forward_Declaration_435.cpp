//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_With_Forward_Declaration_435
 */ 
class MutatorFrontendAction_435 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(435)

private:
    class MutatorASTConsumer_435 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_435(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl *> forwardDeclaredEnums;
    };
};

//source file
#include "../include/enum_with_forward_declaration_435.h"

// ========================================================================================================
#define MUT435_OUTPUT 1

void MutatorFrontendAction_435::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;
        
        if (ED->isCompleteDefinition())
            return;

        forwardDeclaredEnums.push_back(ED);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (forwardDeclaredEnums.empty())
            return;

        auto enumName = forwardDeclaredEnums.back()->getNameAsString();
        std::string newSignature;
        if (FD->getNumParams() > 0) {
            newSignature = FD->getNameInfo().getAsString() + "(" + enumName + " value, ";
            for (unsigned i = 0; i < FD->getNumParams(); ++i) {
                if (i > 0)
                    newSignature += ", ";
                newSignature += FD->parameters()[i]->getType().getAsString() + " " + FD->parameters()[i]->getNameAsString();
            }
            newSignature += ")";
        } else {
            newSignature = FD->getNameInfo().getAsString() + "(" + enumName + " value)";
        }

        auto functionRange = FD->getSourceRange();
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), functionRange);
        auto pos = functionText.find(FD->getNameInfo().getAsString());
        if (pos != std::string::npos) {
            functionText.replace(pos, FD->getNameInfo().getAsString().length(), newSignature);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionRange), functionText);
        }
    }
}

void MutatorFrontendAction_435::MutatorASTConsumer_435::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl(unless(isDefinition())).bind("EnumDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}