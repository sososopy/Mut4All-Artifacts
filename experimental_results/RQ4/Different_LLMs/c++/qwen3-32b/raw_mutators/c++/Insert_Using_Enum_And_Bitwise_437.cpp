//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Using_Enum_And_Bitwise_437
 */ 
class MutatorFrontendAction_437 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(437)

private:
    class MutatorASTConsumer_437 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_437(Rewriter &R) : TheRewriter(R) {}
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
        std::string className;
        std::string enumName;
        std::vector<std::string> enumerators;
        bool hasEnumInfo = false;
    };
};

//source file
#include "../include/Insert_Using_Enum_And_Bitwise_437.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        className = RD->getNameAsString();
        for (const auto *Decl : RD->decls()) {
            if (const auto *ED = dyn_cast<clang::EnumDecl>(Decl)) {
                enumName = ED->getNameAsString();
                for (const auto *Enumerator : ED->enumerators()) {
                    enumerators.push_back(Enumerator->getNameAsString());
                }
                if (enumerators.size() >= 2) {
                    hasEnumInfo = true;
                    break;
                }
            }
        }

        if (hasEnumInfo) {
            std::string usingEnum = "using enum " + className + "::" + enumName + ";";
            usingEnum = "/*mut437*/" + usingEnum;
            SourceLocation endLoc = RD->getEndLoc();
            Rewrite.InsertTextAfter(endLoc, "\n" + usingEnum);
        }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (!hasEnumInfo || enumerators.size() < 2)
            return;

        if (VD->hasInit()) {
            auto init = VD->getInit();
            if (auto *IL = dyn_cast<clang::IntegerLiteral>(init)) {
                if (IL->getValue().isZero()) {
                    std::string expr = "(" + enumerators[0] + " | " + enumerators[1] + ")";
                    std::string replacement = "/*mut437*/" + expr;
                    SourceLocation startLoc = VD->getInit()->getSourceRange().getBegin();
                    SourceLocation endLoc = VD->getInit()->getSourceRange().getEnd();
                    Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
                }
            }
        }
    }
}

void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classMatcher = cxxRecordDecl(has(enumDecl())).bind("record");
    DeclarationMatcher varMatcher = varDecl(hasType(isInteger()), hasInitializer(integerLiteral(equals(0)))).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}