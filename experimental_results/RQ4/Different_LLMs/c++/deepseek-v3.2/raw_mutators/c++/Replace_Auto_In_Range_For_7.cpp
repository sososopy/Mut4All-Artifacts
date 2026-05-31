//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_In_Range_For_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> insertedPlaceholderTypes;
    };
};

//source file
#include "../include/Replace_Auto_In_Range_For_7.h"

#define MUT1_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("ForStmt")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
            return;

        auto *RangeFor = FS->getInit();
        if (!RangeFor || !RangeFor->getStmtClass() == clang::Stmt::DeclStmtClass)
            return;

        auto *DeclStmt = static_cast<clang::DeclStmt*>(RangeFor);
        if (DeclStmt->isSingleDecl()) {
            auto *Decl = DeclStmt->getSingleDecl();
            if (Decl->getKind() == clang::Decl::Var) {
                auto *VarDecl = static_cast<clang::VarDecl*>(Decl);
                auto Type = VarDecl->getType();
                if (Type->isAutoType()) {
                    auto *RangeExpr = FS->getRangeInit();
                    if (!RangeExpr)
                        return;

                    QualType RangeType = RangeExpr->getType();
                    QualType ElementType;

                    if (RangeType->isArrayType()) {
                        ElementType = RangeType->getAsArrayTypeUnsafe()->getElementType();
                    } else if (RangeType->isRecordType()) {
                        auto *RecordDecl = RangeType->getAsRecordDecl();
                        if (RecordDecl) {
                            for (auto *Field : RecordDecl->fields()) {
                                if (Field->getNameAsString() == "value_type") {
                                    ElementType = Field->getType();
                                    break;
                                }
                            }
                        }
                    } else if (RangeExpr->getStmtClass() == clang::Stmt::InitListExprClass) {
                        auto *InitList = static_cast<clang::InitListExpr*>(RangeExpr);
                        if (InitList->getNumInits() > 0) {
                            ElementType = InitList->getInit(0)->getType();
                        }
                    }

                    if (ElementType.isNull()) {
                        std::string PlaceholderType = "placeholder_type";
                        if (insertedPlaceholderTypes.find(PlaceholderType) == insertedPlaceholderTypes.end()) {
                            Rewrite.InsertTextBefore(Result.Context->getTranslationUnitDecl()->getBeginLoc(), "struct " + PlaceholderType + ";\n");
                            insertedPlaceholderTypes.insert(PlaceholderType);
                        }
                        ElementType = Result.Context->getRecordType(Result.Context->getRecordDecl(Result.Context->getTranslationUnitDecl()));
                    }

                    std::string TypeString = ElementType.getAsString();
                    std::string VarName = VarDecl->getNameAsString();
                    std::string OriginalText = stringutils::rangetoStr(*Result.SourceManager, FS->getSourceRange());
                    std::string ReplacementText = OriginalText;
                    size_t AutoPos = ReplacementText.find("auto");
                    if (AutoPos != std::string::npos) {
                        ReplacementText.replace(AutoPos, 4, TypeString);
                        ReplacementText = "/*mut1*/" + ReplacementText;
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FS->getSourceRange()), ReplacementText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = forStmt().bind("ForStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}