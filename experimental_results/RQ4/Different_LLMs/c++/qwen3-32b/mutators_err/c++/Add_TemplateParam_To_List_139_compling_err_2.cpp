//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_TemplateParam_To_List_139
 */ 
class MutatorFrontendAction_139 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(139)

private:
    class MutatorASTConsumer_139 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_139(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_TemplateParam_To_List_139.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *usingDecl = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
        if (!usingDecl || !Result.Context->getSourceManager().isWrittenInMainFile(usingDecl->getLocation()))
            return;

        QualType T = usingDecl->getType();
        if (auto *TST = T->getAs<clang::TemplateSpecializationType>()) {
            if (TST->getTemplateName().getAsIdentifierInfo()->getName() == "TemplatedClass" && TST->getNumArgs() == 2) {
                const clang::TemplateArgument &KeyArg = TST->getArg(0);
                const clang::TemplateArgument &ListArg = TST->getArg(1);

                SourceRange KeyRange = KeyArg.getAsExpr()->getSourceRange();
                SourceRange ListRange = ListArg.getAsExpr()->getSourceRange();

                std::string KeyText = stringutils::rangetoStr(*Result.SourceManager, KeyRange);
                std::string ListText = stringutils::rangetoStr(*Result.SourceManager, ListRange);

                if (ListText.find("List<") == 0) {
                    std::string NewListText = "List<" + KeyText + ", " + ListText.substr(5);
                    Rewrite.ReplaceText(ListRange, NewListText);
                }
            }
        }
    }
}

void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl(
        hasAnyDeclaration(
            clang::ast_matchers::typeAliasDecl(
                hasType(
                    clang::ast_matchers::templateSpecializationType(
                        hasName("TemplatedClass"),
                        has(clang::ast_matchers::templateArgs(
                            clang::ast_matchers::templateArgument(
                                clang::ast_matchers::isParameterPack()
                            ),
                            clang::ast_matchers::templateArgument(
                                clang::ast_matchers::templateSpecializationType(
                                    hasName("List"),
                                    has(clang::ast_matchers::parameterPack())
                                )
                            )
                        ))
                    )
                )
            )
        )
    ).bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}