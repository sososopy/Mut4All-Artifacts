//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_83
 */ 
class MutatorFrontendAction_83 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(83)

private:
    class MutatorASTConsumer_83 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_83(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> baseClasses;
    };
};

//source file
#include "../include/insert_virtual_base_class_83.h"

// ========================================================================================================
#define MUT83_OUTPUT 1

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if ((!RD->isStruct() && !RD->isClass()) || RD->isLambda())
            return;
        if (!RD->isCompleteDefinition())
            return;
        baseClasses.insert(RD);
    } else if (auto *DD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation()))
            return;
        if ((!DD->isStruct() && !DD->isClass()) || DD->isLambda())
            return;
        if (!DD->isCompleteDefinition())
            return;

        for (const auto *Base : baseClasses) {
            if (!DD->isDerivedFrom(Base)) {
                std::string baseName = Base->getNameAsString();
                std::string derivedName = DD->getNameAsString();
                std::string insertion = "\n/*mut83*/ class DerivedFrom" + baseName + " : virtual public " + baseName + " {\npublic:\n    DerivedFrom" + baseName + "() {}\n};\n";
                Rewrite.InsertText(DD->getEndLoc(), insertion);
                llvm::outs() << "Inserted virtual base class for " << derivedName << " from " << baseName << "\n";
                break;
            }
        }
    }
}

void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}