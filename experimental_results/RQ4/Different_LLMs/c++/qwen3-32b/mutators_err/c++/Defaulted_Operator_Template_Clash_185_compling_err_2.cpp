//header file
#pragma once
#include "Mutator_base.h"

/**
 * Defaulted_Operator_Template_Clash_185
 */ 
class MutatorFrontendAction_185 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(185)

private:
    class MutatorASTConsumer_185 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_185(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FieldDecl *> targetFields;
    };
};

//source file
#include "../include/Defaulted_Operator_Template_Clash_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("TargetField")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        auto *RD = FD->getParent();
        auto *Ctx = Result.Context;
        clang::SourceManager *SM = &Ctx->getSourceManager();

        // Check if struct has defaulted operator==
        bool hasDefaultedOp = false;
        for (const auto *D : RD->decls()) {
            if (const auto *FD = dyn_cast<clang::FriendDecl>(D)) {
                if (const auto *FD2 = FD->getFriendDecl()) {
                    if (const auto *FD3 = dyn_cast<clang::FunctionDecl>(FD2)) {
                        if (FD3->isDefaulted()) {
                            hasDefaultedOp = true;
                            break;
                        }
                    }
                }
            }
        }

        if (!hasDefaultedOp) return;

        // Check if field type is non-templated
        auto type = FD->getType();
        if (type->getAs<clang::TemplateSpecializationType>()) return;

        // Get source ranges
        auto fieldRange = FD->getSourceRange();
        auto structRange = RD->getSourceRange();
        auto fileID = SM->getMainFileID();
        auto fileStart = SM->getLocForStartOfFile(fileID);

        // Replace field type with T<int>
        std::string replacement = "T<int>";
        Rewrite.ReplaceText(fieldRange, replacement);

        // Insert template struct T
        std::string templateDecl = "template <typename> struct T {};\n";
        Rewrite.InsertText(fileStart, templateDecl);

        // Insert conflicting operator==
        std::string opDecl = "template <typename T1, typename T2> constexpr bool operator==(T<T1>, T<T2> x) { return 0 == x; }\n";
        Rewrite.InsertTextAfterToken(RD->getEndLoc(), opDecl);
    }
}
  
void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        has(fieldDecl(hasType(unless(isTemplateType()))).bind("TargetField"))
    ).bind("TargetStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}