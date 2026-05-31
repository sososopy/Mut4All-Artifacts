//source file
#include "../include/Anonymous_Struct_Usage_With_Alias_Template_371.h"

// ========================================================================================================
#define MUT371_OUTPUT 1

void MutatorFrontendAction_371::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AliasDecl = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplate")) {
        if (!AliasDecl || !Result.Context->getSourceManager().isWrittenInMainFile(AliasDecl->getLocation()))
            return;

        auto aliasName = AliasDecl->getNameAsString();
        SourceLocation insertLoc = AliasDecl->getEndLoc().getLocWithOffset(1);

        std::string structUsage = "\n/*mut371*/{\n    struct {\n        " + aliasName + " memberVariable;\n    };\n}\n";
        Rewrite.InsertTextAfter(insertLoc, structUsage);
    }
}

void MutatorFrontendAction_371::MutatorASTConsumer_371::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}