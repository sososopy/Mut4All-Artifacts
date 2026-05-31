//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Constructor_Specialization_4
 */ 
class MutatorFrontendAction_4 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(4)

private:
    class MutatorASTConsumer_4 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_4(Rewriter &R) : TheRewriter(R) {}
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

//souce file
//source file
#include "../include/Add_Explicit_Constructor_Specialization_4.h"

// ========================================================================================================
#define MUT4_OUTPUT 1

void MutatorFrontendAction_4::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!derived || !Result.Context->getSourceManager().isWrittenInMainFile(derived->getLocation()))
            return;
        if (!derived->isCompleteDefinition())
            return;

        for (const auto &base : derived->bases()) {
            const auto *baseType = base.getType()->getAsCXXRecordDecl();
            if (!baseType)
                continue;

            for (const auto *ctor : baseType->ctors()) {
                if (ctor->isUserProvided()) continue;

                std::string paramList;
                for (unsigned i = 0; i < ctor->getNumParams(); ++i) {
                    const auto *param = ctor->getParamDecl(i);
                    std::string typeName = param->getType().getAsString();
                    std::string paramName = "arg" + std::to_string(i + 1);
                    if (i > 0) paramList += ", ";
                    paramList += typeName + " " + paramName;
                }

                std::string constructorName = derived->getNameAsString();
                std::string newCtor = "explicit " + constructorName + "(" + paramList + ") : " + baseType->getNameAsString() + "(" + paramList + ") {}";

                SourceLocation endLoc = derived->getEndLoc();
                if (endLoc.isInvalid())
                    continue;

                newCtor = "\n" + newCtor;
                Rewrite.InsertText(endLoc, newCtor);
            }
        }
    }
}
  
void MutatorFrontendAction_4::MutatorASTConsumer_4::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(usingDecl(hasTarget(ctorDecl())))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}