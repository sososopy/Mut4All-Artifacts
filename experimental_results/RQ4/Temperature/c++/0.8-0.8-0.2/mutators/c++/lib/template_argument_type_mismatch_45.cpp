//source file
#include "../include/template_argument_type_mismatch_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templateDecl")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;
        TemplateDecl = CTD;
    }

    if (auto *CTS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("templateSpec")) {
        if (!CTS || !Result.Context->getSourceManager().isWrittenInMainFile(CTS->getLocation()))
            return;
        
        if (!TemplateDecl || TemplateDecl != CTS->getSpecializedTemplate())
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto range = CTS->getSourceRange();
        auto content = stringutils::rangetoStr(SM, range);

        //Perform mutation on the source code text by applying string replacement
        unsigned paramCount = CTS->getTemplateArgs().size();
        if (paramCount > 0) {
            for (unsigned i = 0; i < paramCount; ++i) {
                const clang::TemplateArgument &arg = CTS->getTemplateArgs().get(i);
                if (arg.getKind() == clang::TemplateArgument::Type) {
                    std::string oldType = arg.getAsType().getAsString();
                    std::string newType = "void";
                    size_t pos = content.find(oldType);
                    if (pos != std::string::npos) {
                        content.replace(pos, oldType.length(), newType);
                        break; // Modify only one template argument
                    }
                }
            }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), content);
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("templateDecl");
    DeclarationMatcher specializationMatcher = classTemplateSpecializationDecl().bind("templateSpec");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(specializationMatcher, &callback);
    matchFinder.matchAST(Context);
}