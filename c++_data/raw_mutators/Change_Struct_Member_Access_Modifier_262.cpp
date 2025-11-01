//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Struct_Member_Access_Modifier_262
 */ 
class MutatorFrontendAction_262 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(262)

private:
    class MutatorASTConsumer_262 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_262(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::CXXRecordDecl*, std::set<clang::AccessSpecifier>> classAccessSpecifiers;
    };
};

//source file
#include "../include/Change_Struct_Member_Access_Modifier_262.h"

// ========================================================================================================
#define MUT262_OUTPUT 1

void MutatorFrontendAction_262::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        if (!RD->isCompleteDefinition())
            return;

        if (classAccessSpecifiers.find(RD) == classAccessSpecifiers.end()) {
            std::set<clang::AccessSpecifier> accessSpecifiers;
            for (const auto *D : RD->decls()) {
                if (const auto *FD = llvm::dyn_cast<clang::FieldDecl>(D)) {
                    accessSpecifiers.insert(FD->getAccess());
                }
                if (const auto *MD = llvm::dyn_cast<clang::CXXMethodDecl>(D)) {
                    accessSpecifiers.insert(MD->getAccess());
                }
            }
            classAccessSpecifiers[RD] = accessSpecifiers;
        }

        for (const auto *D : RD->decls()) {
            clang::AccessSpecifier currentAccess;
            if (const auto *FD = llvm::dyn_cast<clang::FieldDecl>(D)) {
                currentAccess = FD->getAccess();
            } else if (const auto *MD = llvm::dyn_cast<clang::CXXMethodDecl>(D)) {
                currentAccess = MD->getAccess();
            } else {
                continue;
            }

            clang::AccessSpecifier newAccess = clang::AS_none;
            if (currentAccess != clang::AS_public && classAccessSpecifiers[RD].count(clang::AS_public) == 0) {
                newAccess = clang::AS_public;
            } else if (currentAccess != clang::AS_private && classAccessSpecifiers[RD].count(clang::AS_private) == 0) {
                newAccess = clang::AS_private;
            } else if (currentAccess != clang::AS_protected && classAccessSpecifiers[RD].count(clang::AS_protected) == 0) {
                newAccess = clang::AS_protected;
            }

            if (newAccess != clang::AS_none) {
                std::string newAccessStr;
                switch (newAccess) {
                    case clang::AS_public: newAccessStr = "public"; break;
                    case clang::AS_private: newAccessStr = "private"; break;
                    case clang::AS_protected: newAccessStr = "protected"; break;
                    default: break;
                }

                SourceLocation startLoc = D->getBeginLoc();
                SourceLocation endLoc = D->getEndLoc();
                if (currentAccess != clang::AS_none) {
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), newAccessStr + ": " + Rewrite.getRewrittenText(CharSourceRange::getTokenRange(startLoc, endLoc)));
                } else {
                    Rewrite.InsertTextBefore(startLoc, newAccessStr + ": ");
                }
                classAccessSpecifiers[RD].insert(newAccess);
            }
        }
    }
}

void MutatorFrontendAction_262::MutatorASTConsumer_262::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}